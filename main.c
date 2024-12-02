//
// Created by lennart on 12/2/24.
//


#include <stdio.h>
#include <stdlib.h>

extern void run_day1();
extern void run_day2();

int main(int argc, char **argv) {
    int day = strtod(argv[1], nullptr);
    switch (day) {
    case 1: run_day1(); break;
    case 2: run_day2(); break;
        default:
            fprintf(stderr, "Not implemented day %d yet\n");
    }
}
