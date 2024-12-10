#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(FILE *f) {
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

    return data;
}

int data_linecount(char* data, size_t data_len) {
    int numlines = 0;
    unsigned int pos = 0;
    while (pos < data_len) {
        if (data[pos] == '\n') {
            numlines++;
        }
        pos++;
    }
    return numlines;
}

int compare_int(int *a, int *b) {
    return *a - *b;
}

int run_day1(void) {
    // Read file data
    FILE *f = fopen("input.txt", "r");
    char *data = read_file(f);
    size_t data_len = strlen(data);

    // Count number of lines (= list size)
    int listSize = data_linecount(data, data_len);

    // Allocate our lists
    int *L1 = calloc(listSize, sizeof(int));
    int *L2 = calloc(listSize, sizeof(int));

    // Read raw text data into lists
    int position = 0;
    for(int i = 0; i < listSize; i++) {
        sscanf(&data[position], "%d   %d", &L1[i], &L2[i]);
        while (data[position] != 0 && data[position++] != '\n');
    }

    // Sort lists
    qsort(L1, listSize, sizeof(int), (__compar_fn_t) compare_int);
    qsort(L2, listSize, sizeof(int), (__compar_fn_t) compare_int);

    // Sum distances
    int result = 0;
    for (int i = 0; i < listSize; i++) {
        result += abs(L1[i] - L2[i]);
    }
    printf("Result #1: %d\n", result);

    // Sum similarity scores
    result = 0;
    for (int i = 0; i < listSize; i++) {
        int similarity = 0;
        int candidate = L1[i];

        for (int j = 0; j < listSize; j++) {
            if (L2[j] == candidate) similarity++;
        }

        result += candidate * similarity;
    }
    printf("Result #2: %d\n", result);

    free(L1); free(L2);
    free(data);
    fclose(f);
    return 0;
}
