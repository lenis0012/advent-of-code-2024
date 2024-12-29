//
// Created by lennart on 12/2/24.
//

/*
 * Basic AOC Helper functions
 * for parsing inputs
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

void aoc_begin();
void aoc_end();
char aoc_char();
void aoc_unget(char c);
bool aoc_eof();
int aoc_scan(const char *format, ...);
void aoc_rewind();
void aoc_expect(const char *str);
bool aoc_parse(const char* str);
void aoc_whitespace();
int aoc_eat(const char *pattern);
char *aoc_line(char *buffer, int length);
int aoc_int();
bool aoc_parse_int(int *out);

// Maps
typedef struct {
    char *cells;
    unsigned int width;
    unsigned int height;
} Map;

Map *aoc_map_load();
char aoc_map(Map *map, int x, int y);
void aoc_map_print(Map *map);
void aoc_map_set(Map *map, int x, int y, char cell);

static char* aoc_read_file(FILE *f) {
    if (f == NULL) {
        fprintf(stderr, "Couln't find input file\n");
        exit(EXIT_FAILURE);
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    rewind(f);

    char *data = malloc(fsize + 1);
    fread(data, sizeof(char), fsize, f);
    data[fsize] = '\0';

    return data;
}

static unsigned int aoc_file_size(FILE *f) {
    fpos_t pos;
    fgetpos(f, &pos);

    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);

    fsetpos(f, &pos);
    return fsize;
}
