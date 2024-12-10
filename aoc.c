#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc.h"

static FILE *f = nullptr;

void aoc_begin() {
    f = fopen("input.txt", "r");
}

char aoc_char() {
    return (char) fgetc(f);
}

void aoc_expect(const char *str) {
    while (*str != 0) {
        assert(fgetc(f) == *(str++));
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

char *aoc_line(char *buffer, long int length) {
    fpos_t mark; fgetpos(f, &mark);

    if (buffer == nullptr) {
        long int start = ftell(f);
        int c;
        while ((c = fgetc(f)) != EOF && c != '\n') {}
        length = ftell(f) - start + 1; // Reserve for \0
        fsetpos(f, &mark);

        buffer = malloc(length);
    }

    // Read the line into buffer and remove newline character(s)
    assert(fgets(buffer, length, f) != nullptr);
    if (buffer[length - 1] == '\n') buffer[length - 1] = '\0';
    if (buffer[length - 2] == '\r') buffer[length - 2] = '\0';
    return buffer;
}

int aoc_int() {
    assert(!feof(f));

    // Read first digit
    int c = fgetc(f);
    while (c == ' ') {
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

    return result;
}

bool aoc_parse_int(int *out) {
    fpos_t mark; fgetpos(f, &mark);

    // Read first non-whitespace char
    int c;
    do {
        c = fgetc(f);
    } while (c == ' ');;

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

    *out = result;
    return true;
}

void aoc_end() {
    fclose(f);
}
