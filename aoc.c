#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "aoc.h"

static FILE *f = nullptr;

char aoc_char() {
    return (char) fgetc(f);
}

void aoc_unget(char c) {
    ungetc(c, f);
}

bool aoc_eof() {
    return feof(f);
}

int aoc_scan(const char *format, ...) {
    va_list vargs;
    va_start(vargs);
    vfscanf(f, format, vargs);
    va_end(vargs);
}

void aoc_rewind() {
    fseek(f, 0, SEEK_SET);
}

void aoc_expect(const char *str) {
    while (*str != 0) {
        int c = fgetc(f);
        assert(c == *str);
        str++;
    }
}

bool aoc_parse(const char* str) {
    fpos_t mark; fgetpos(f, &mark);

    while (*str != 0) {
        if (fgetc(f) != *(str++)) {
            fsetpos(f, &mark);
            return false;
        }
    }

    return true;
}

void aoc_whitespace() {
    int c;
    do {
        c = fgetc(f);
    } while (c == ' ');
    ungetc(c, f);
}

int aoc_eat(const char *pattern) {
    int c, i = 0;
    while ((c = fgetc(f)) != EOF && strchr(pattern, c) != nullptr) {
        i++;
    };
    ungetc(c, f);

    return i;
}

char *aoc_line(char *buffer, int length) {
    fpos_t mark; fgetpos(f, &mark);

    if (buffer == nullptr) {
        int start = (int) ftell(f);
        int c = 0;
        while ((c = fgetc(f)) != EOF && c != '\n') {}
        length = ftell(f) - start + 1; // Reserve for \0
        fsetpos(f, &mark);

        if (length > 1) buffer = malloc(length);
    }

    // Read the line into buffer and remove newline character(s)
    if(buffer == nullptr || fgets(buffer, length, f) == nullptr) {
        return nullptr;
    }
    int strlength = strlen(buffer);
    if (buffer[strlength - 1] == '\n') buffer[strlength - 1] = '\0';
    if (buffer[strlength - 2] == '\r') buffer[strlength - 2] = '\0';
    return buffer;
}

int aoc_int() {
    assert(!feof(f));

    // Read first digit
    int c = fgetc(f);
    while (c == ' ') {
        c = fgetc(f);
    }

    bool negative = false;
    if (c == '-') {
        negative = true;
        c = fgetc(f);
    }
    assert(c >= '0' && c <= '9');

    // Process digits
    int result = 0;
    do {
        if (c < '0' || c > '9') {
            ungetc(c, f);
            break;
        }

        result = result * 10 + (c - '0');
    } while ((c = fgetc(f)) != EOF);

    return negative ? result * -1 : result;
}

bool aoc_parse_int(int *out) {
    fpos_t mark; fgetpos(f, &mark);

    // Read first non-whitespace char
    int c;
    do {
        c = fgetc(f);
    } while (c == ' ');;

    bool negative = false;
    if (c == '-') {
        negative = true;
        c = fgetc(f);
    }

    // Validate
    if (c < '0' || c > '9') {
        fsetpos(f, &mark);
        return false;
    }

    // Read number
    int result = 0;
    do {
        result = result * 10 + (c - '0');
    } while ((c = fgetc(f)) != EOF && c >= '0' && c <= '9');

    *out = negative ? result * -1 : result;
    return true;
}

//region Maps
#define LIST_NAME maps
#define LIST_ELEMENT Map*
#include "list.h"

static maps_t *maps;

Map* aoc_map_load() {
    if (feof(f)) return nullptr;

    Map *map = malloc(sizeof(Map));
    fpos_t fpos; fgetpos(f, &fpos);

    // Determine width
    map->width = 0;
    int c;
    while ((c = fgetc(f)) >= 32 && c <= 127) {
        map->width++;
    }

    // Determine the height
    map->height = 0;
    while (c != '\n' && c != EOF) {
        c = fgetc(f);
    }
    size_t width;
    do {
        char linebuffer[map->width + 3];
        map->height++;

        if (feof(f)) break;

        fgets(linebuffer, map->width + 3, f);
        width = strlen(linebuffer);
        if (linebuffer[width - 1] == '\n') {
            width--;
            if (linebuffer[width - 2] == '\r') {
                width--;
            }
        }

    } while (width == map->width);

    // Read data
    map->cells = malloc(map->width * map->height);
    fsetpos(f, &fpos);
    for (int i = 0; i < map->width * map->height; i++) {
        int c = fgetc(f);
        assert(c != EOF);
        if (c < 32 || c > 127) {
            i--;
            continue;
        }

        map->cells[i] = (char) c;
    }

    // Read end of line
    do { c = fgetc(f); } while (c != '\n' && c != EOF);

    return map;
}

char aoc_map(Map *map, int x, int y) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height) {
        return 0;
    }

    return map->cells[y * map->width + x];
}

void aoc_map_set(Map *map, int x, int y, char cell) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height) {
        return;
    }

    map->cells[y * map->width + x] = cell;
}

void aoc_map_print(Map *map) {
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            printf("%c", aoc_map(map, x, y));
        }
        printf("\n");
    }
}
//endregion

void aoc_begin() {
    f = fopen("input.txt", "r");
    maps = maps_new();
}

void aoc_end() {
    fclose(f);
    for (int i = 0; i < maps->size; i++) {
        free(maps_get(maps, i)->cells);
        free(maps_get(maps, i));
    }
    maps_destroy(maps);
}
