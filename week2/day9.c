//
// Created by lennart on 12/9/24.
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int32_t fid_t; // File ID

static fid_t *blocks;
static unsigned int nblocks = 0, ncapacity = 1024;

#define FID_EMPTY (-1)

static void print_blocks() {
    for (int i = 0; i < nblocks; i++) {
        if (blocks[i] == FID_EMPTY) printf(".");
        else printf("%u ", blocks[i]);
    }
    printf("\n");
}

static void load_data(void) {
    FILE *f = fopen("input.txt", "r");

    blocks = malloc(ncapacity * sizeof(fid_t));
    fid_t fid = 0;
    nblocks = 0;
    while (!feof(f)) {
        uint8_t size = fgetc(f) - '0';
        if (ncapacity <= nblocks + size) {
            ncapacity *= 2;
            blocks = realloc(blocks, ncapacity * sizeof(fid_t));
        }

        for (int i  = 0; i < size; i++) {
            blocks[nblocks++] = fid;
        }
        fid++;

        int c = fgetc(f);
        if (c != '\n' && c != EOF) {
            uint8_t empty = c - '0';
            if (ncapacity <= nblocks + empty) {
                ncapacity *= 2;
                blocks = realloc(blocks, ncapacity * sizeof(fid_t));
            }
            for (int i  = 0; i < empty; i++) {
                blocks[nblocks++] = FID_EMPTY;
            }
        }
    }
    fclose(f);
}

static void part1(void) {
    int free = 0;
    for (int i = nblocks - 1; i > 1; i--) {
        if (blocks[i] == FID_EMPTY) continue;
        while (free < i && blocks[free] != FID_EMPTY) {
            free++;
        }
        if (free >= i) break;

        blocks[free++] = blocks[i];
        blocks[i] = FID_EMPTY;
    }

    uint64_t result = 0;
    for (int i = 0; i < nblocks; i++) {
        if (blocks[i] == FID_EMPTY) continue;
        result += i * blocks[i];
    }
    printf("Result 1: %llu\n", result);
}

static void part2(void) {
    // print_blocks();
    for (int i = nblocks - 1; i > 1; i--) {
        if (blocks[i] == FID_EMPTY) continue;

        fid_t fid = blocks[i];
        int count = 0;
        while (i - count > 0 && blocks[i - ++count] == fid) {}

        int destination = -1;
        for (int j = 0; j < i; j++) {
            if (blocks[j] != FID_EMPTY) continue;
            int k;
            for (k = j; k < j + count; k++) {
                if (blocks[k] != FID_EMPTY) {
                    break;
                }
            }

            if (k - j >= count) {
                destination = j; break;
            }
        }

        if (destination == -1) {
            i -= count - 1;
            continue;
        }

        for (int j = i; j > i - count; j--) {
            blocks[j] = FID_EMPTY;
        }

        // memset(&blocks[i - count + 1], 0, &blocks[i+1] - &blocks[i - count + 1]);
        for (int j = destination; j < destination + count; j++) {
            blocks[j] = fid;
        }
    }

    uint64_t result = 0;
    for (int i = 0; i < nblocks; i++) {
        if (blocks[i] == FID_EMPTY) continue;
        result += i * blocks[i];
    }
    printf("Result 2: %llu\n", result);
}

void run_day9() {
    load_data();
    part1();

    free(blocks);
    load_data();
    part2();
    // print_blocks();
}
