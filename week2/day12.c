//
// Created by lennart on 12/13/24.
//

#include <aoc.h>

typedef struct {
    int x;
    int y;
} Position;

#define LIST_NAME positions
#define LIST_ELEMENT Position
#include "list.h"

typedef struct {
    positions_t positions;
    int id;
    char flower;
    int area;
    int perimiter;
    int perimiter2;
} Region;

#define LIST_NAME regions
#define LIST_ELEMENT Region
#include "list.h"


static Map *map;
static regions_t *regions;
static int **plotted;

static void find_region(Region *region, int x, int y) {
    // Check type
    char flower = aoc_map(map, x, y);
    if (flower != region->flower) {
        region->perimiter++; // Part 1
        return;
    }

    // Prevent infinite recursion
    if (plotted[x][y] == region->id) {
        return;
    }

    plotted[x][y] = region->id;
    positions_add(&region->positions, (Position) { x, y });
    region->area++;

    Position position = (Position) { x, y };
    for (int i = 0; i < 4; i++) {
        Position npos = position;
        switch (i) {
        case 0: npos.y--; break;
        case 1: npos.x++; break;
        case 2: npos.y++; break;
        case 3: npos.x--; break;
        }

        find_region(region, npos.x, npos.y);
    }
}

static int shape_at(int x, int y) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height) {
        return 0;
    }
    return plotted[x][y];
}

static void get_edges(Region *region) {
    for (int i = 0; i < region->positions.size; i++) {
        Position position = positions_get(&region->positions, i);
        int x = position.x, y = position.y;

        // Count edges by only counting top-most and left-most plots along edge.
        // Along edge when left or top neighbor not in shape or edge neighboring shape itself
        int ln = shape_at(x - 1, y);
        int tn = shape_at(x, y - 1);
        int tln = shape_at(x - 1, y - 1);
        int rn = shape_at(x + 1, y);
        int trn = shape_at(x + 1, y - 1);
        int bn = shape_at(x, y + 1);
        int bln = shape_at(x - 1, y + 1);

        // Ledge edge
        if (ln != region->id && (tn != region->id || tln == region->id)) {
            region->perimiter2++;
        }
        // Right edge
        if (rn != region->id && (tn != region->id || trn == region->id)) {
            region->perimiter2++;
        }
        // Top edge
        if (tn != region->id && (ln != region->id || tln == region->id)) {
            region->perimiter2++;
        }
        // Bottom edge
        if (bn != region->id && (ln != region->id || bln == region->id)) {
            region->perimiter2++;
        }
    }
}

void run_day12() {
    aoc_begin();

    map = aoc_map_load();

    plotted = malloc(map->width * sizeof(int*));
    for (int i = 0; i < map->width; i++) {
        plotted[i] = calloc(sizeof(int), map->height);
    }

    regions = regions_new();
    for (int x  = 0; x < map->width; x++) {
        for (int y = 0; y < map->height; y++) {
            if (plotted[x][y]) {
                continue;
            }

            Region region = {
                .id = (int) regions->size + 1,
                .flower = aoc_map(map, x, y)
            };
            positions_init(&region.positions);
            find_region(&region, x, y);
            get_edges(&region);
            positions_fini(&region.positions);
            regions_add(regions, region);
            // printf("Region %c = %d with perimiter %d\n", region.flower, region.area, region.perimiter);
        }
    }

    // Part 1
    int result = 0;
    for (int i = 0; i < regions->size; i++) {
        Region *region = &regions->elements[i];
        result += region->area * region->perimiter;
    }
    printf("Result: %d\n", result);

    // Part 2
    result = 0;
    for (int i = 0; i < regions->size; i++) {
        Region *region = &regions->elements[i];
        result += region->area * region->perimiter2;
    }
    printf("Result: %d\n", result);

    // Cleanup
    regions_destroy(regions);
    for (int i = 0; i < map->width; i++) { free(plotted[i]); }
    free(plotted);
    aoc_end();
}
