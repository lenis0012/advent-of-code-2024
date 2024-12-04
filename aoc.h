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
