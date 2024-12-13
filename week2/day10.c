//
// Created by lennart on 12/10/24.
//

#include <aoc.h>

#define LIST_NAME lines
#define LIST_ELEMENT char*
#include "list.h"

typedef struct {
    short x;
    short y;
} Peak;

#define LIST_NAME peaks
#define LIST_ELEMENT Peak
#define LIST_EQ(a, b) (a.x == b.x && a.y == b.y)
#include "list.h"

static char **map;
static unsigned int width;
static unsigned int height;
static bool distinct = true;

static char map_at(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return 0;

    return map[y][x];
}

int is_new_peak(int y, int x, peaks_t *peaks) {
    if (y < 0 || y >= height || x < 0 || x >= width || map[y][x] != '9') return 0;

    if (!distinct) return 1;

    Peak p = (Peak) { (short) x, (short)  y };
    if (peaks_index_of(peaks, p) == -1) {
        peaks_add(peaks, p);
        return 1;
    }
    return 0;
}

static int count_trails(int y, int x, char c, peaks_t *peaks) {
    char next = (char) (c + 1);

    int result = 0;
    if (next == '9') {
        result += is_new_peak(y - 1, x, peaks);
        result += is_new_peak(y, x + 1, peaks);
        result += is_new_peak(y + 1, x, peaks);
        result += is_new_peak(y, x - 1, peaks);
    } else {
        if (map_at(x, y - 1) == next) result += count_trails(y - 1, x, next, peaks); // North
        if (map_at(x + 1, y) == next) result += count_trails(y, x + 1, next, peaks); // North
        if (map_at(x, y + 1) == next) result += count_trails(y + 1, x, next, peaks); // North
        if (map_at(x - 1, y) == next) result += count_trails(y, x - 1, next, peaks); // North
    }

    return result;
}

void run_day10() {
    aoc_begin();

    lines_t lines; lines_init(&lines);
    char *line;
    while ((line = aoc_line(nullptr,  0)) != nullptr) {
        lines_add(&lines, line);
    }
    map = lines.elements;
    height = lines.size;
    width = strlen(lines_get(&lines, 0));

    // Part 1
    int result = 0;
    peaks_t peaks; peaks_init(&peaks);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x] == '0') {
                peaks.size = 0;
                result += count_trails(y, x, '0', &peaks);
            }
        }
    }
    printf("Result 1: %d\n", result);

    // Part 1
    distinct = false;
    result = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x] == '0') {
                result += count_trails(y, x, '0', &peaks);
            }
        }
    }
    printf("Result 2: %d\n", result);

    peaks_fini(&peaks);
    lines_fini(&lines);
    aoc_end();
}
